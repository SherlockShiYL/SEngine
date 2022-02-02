#include "ServerNetworkManager.h"
#include "ClientNetworkManager.h"
#include "Board.h"
#include "Marble.h"

using namespace S;

bool Init(float deltaTime);
bool Menu(float deltaTime);
bool Play(float deltaTime);
bool RunAsServer(float deltaTime);
void RecieveFromServer(Command& recived);

void CheckCollision();

std::function<bool(float)> Tick = Init;

char* ipAdd{ nullptr };
Input::InputSystem* input{ nullptr };
const uint16_t port = 8888;
Board board[2];
Marble marble;
uint32_t mClientID{ 0 };
uint8_t buffer[16384];
int bytesReceived{ 0 };
Command commandSend, lastSentCommand, commandReci;
Math::Vector2 marbleCurrentVelocity;

int clientNumber{ 0 };
float countDown{ 1.0f };

bool isServer{ false };
bool isClient{ false };


bool Init(float deltaTime)
{
	input = Input::InputSystem::Get();
	Tick = Menu;
	board[0].Load({ 300.0f,100.0f }, "stone.png");
	board[1].Load({ 300.0f,Graphics::GetScreenHeight() - 100.0f }, "qmark.png");
	marble.Load();

	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

void RecieveFromServer(Command& recived)
{
	if (ClientNetworkManager::Get()->HandleMessage(buffer, static_cast<int>(std::size(buffer)), bytesReceived))
	{
		Network::MemoryStream memStream(buffer, bytesReceived);
		Network::StreamReader reader(memStream);

		while (reader.GetRemainingDataSize() > 0)
		{
			uint32_t messageType;
			reader.Read(messageType);
			switch (NetworkMessage(messageType))
			{
			case NetworkMessage::Snapshot:
				break;
			case NetworkMessage::Command:
				uint32_t tempId;
				reader.Read(tempId);
				recived.Deserialize(reader);
				board[tempId].SetCommand(recived);
				LOG("[Client] ID: %u, Command: %u", tempId, recived.mCommand);
				break;
			case NetworkMessage::Marble:
			{
				Math::Vector2 tempV2;
				reader.Read(tempV2.x);
				reader.Read(tempV2.y);
				marble.SetPosition(tempV2);
				reader.Read(tempV2.x);
				reader.Read(tempV2.y);
				marble.SetVelocity(tempV2);
				break;
			}
			default:
				break;
			}
		}
	}
	else
	{
		//gameState = GameState::Disconnected;
	}
}

bool Menu(float deltaTime)
{
	ImGui::InputText("Ip Address:", ipAdd, 20);
	ImGui::Checkbox("Play", &isClient);
	ImGui::Checkbox("Run Server", &isServer);
	if (isClient)
	{
		ClientNetworkManager::StaticInitialize();
		while (!ClientNetworkManager::Get()->ConnectToServer(ipAdd, 8888))
		//while (!ClientNetworkManager::Get()->ConnectToServer("127.0.0.1", 8888))
		//while (!ClientNetworkManager::Get()->ConnectToServer("192.168.0.105", 8888))
		{

		}
			// Get initialize data from server
		
		mClientID = 999u;
		do
		{
			if (ClientNetworkManager::Get()->HandleMessage(buffer, static_cast<int>(std::size(buffer)), bytesReceived))
			{
				Network::MemoryStream memStream(buffer, bytesReceived);
				Network::StreamReader reader(memStream);

				reader.Read(mClientID);
				LOG("[Client Initialize] ID: %u", mClientID);
			}
		} while (bytesReceived <= 0);
		Tick = Play;
	}
	if (isServer)
	{
		marble.InitializeVelocity();
		ServerNetworkManager::StaticInitialize();
		Tick = RunAsServer;
	}
	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

bool Play(float deltaTime)
{
	// Player Input
	commandReci.Clear();
	if (input->IsKeyDown(Input::KeyCode::A))
	{
		commandSend.mCommand |= Command::kLeft;
	}
	if (input->IsKeyDown(Input::KeyCode::D))
	{
		commandSend.mCommand |= Command::kRight;
	}

	if (lastSentCommand.mCommand != commandSend.mCommand)
	{
		Network::MemoryStream memStream;
		Network::StreamWriter writer(memStream);
		// my network id
		writer.Write(NetworkMessage::Command);
		writer.Write(mClientID);
		commandSend.Serialize(writer);
		ClientNetworkManager::Get()->SendMessageToServer(memStream);
		lastSentCommand = commandSend;
		LOG("[Client %u] Command send: %u", mClientID, commandSend.mCommand);
	}
	commandSend.Clear();
	RecieveFromServer(commandReci);

	for (auto& b : board)
	{
		b.Update(deltaTime);
		b.Render();
	}
	marble.Update(deltaTime);
	marble.Render();
	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

bool RunAsServer(float deltaTime)
{
	// Get new client and send to all clients ask for create new character
	uint32_t newClientID{ 0 };
	if (ServerNetworkManager::Get()->HandleNewClients(newClientID))
	{
		Network::MemoryStream memStream;
		Network::StreamWriter writer(memStream);
		// Send players (already exist) info to new client
		//writer.Write(NetworkMessage::Snapshot);
		writer.Write(newClientID);
		LOG("[Server Initialize] ID: %u", newClientID);
		ServerNetworkManager::Get()->SendToClient(newClientID, memStream);
		clientNumber++;
	}


	// Receive all the command from client
	ServerNetworkManager::Get()->ProcessClientMessage(buffer, static_cast<int>(std::size(buffer)), bytesReceived);

	//Network::MemoryStream writerStream;
	//Network::StreamWriter dataWriter(writerStream);

	// Read all the data
	Network::MemoryStream memStream(buffer, bytesReceived);
	Network::StreamReader reader(memStream);
	//printf("Bytes Received: %d\n", bytesReceived);
	while (reader.GetRemainingDataSize() > 0)
	{
		// Receive commands from one client
		uint32_t messageType;
		reader.Read(messageType);

		uint32_t id{ 0 };
		reader.Read(id);

		Command command;
		command.Deserialize(reader);
		board[id].SetCommand(command);
		LOG("[Server] ID: %u, Command R&S: %u", id, command.mCommand);
	}

	// Send all the command to clients
	if (memStream.GetCapacity())
	{
		ServerNetworkManager::Get()->Broadcast(memStream);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	CheckCollision();

	if (clientNumber == 2)
	{
		if (countDown > 0.0f)
			countDown -= deltaTime;
		else
		{
			if (marbleCurrentVelocity.x != marble.GetVelocity().x ||
				marbleCurrentVelocity.y != marble.GetVelocity().y)
			{
				marbleCurrentVelocity = marble.GetVelocity();

				Network::MemoryStream memStream;
				Network::StreamWriter writer(memStream);
				writer.Write(NetworkMessage::Marble);
				writer.Write(marble.GetPosition().x);
				writer.Write(marble.GetPosition().y);
				writer.Write(marble.GetVelocity().x);
				writer.Write(marble.GetVelocity().y);
				ServerNetworkManager::Get()->Broadcast(memStream);
			}



			for (auto& b : board)
			{
				b.Update(deltaTime);
				b.Render();
			}
			marble.Update(deltaTime);
			marble.Render();
		}
	}

	float tempSpeed{ marble.GetSpeed() };
	bool reStart{ false };
	ImGui::DragFloat("Velocity", &tempSpeed, 1.0f, 100.0f, 400.0f);
	ImGui::Checkbox("Restart", &reStart);
	if (reStart)
	{
		marble.SetPosition({ 300.0f,300.0f });
		marble.InitializeVelocity();
	}
	marble.SetSpeed(tempSpeed);
	return input->IsKeyDown(Input::KeyCode::ESCAPE);
}

void CheckCollision()
{
	for (auto& b : board)
	{
		if (Geometry::Intersect(marble.GetCircle(), b.GetRect()))
		{
			Math::Vector2 tempVelocity{ 0.0f,1.0f };
			if (marble.GetVelocity().y > 0.0f)
				tempVelocity.y = -1.0f;
			tempVelocity.x = (marble.GetPosition().x - b.GetPosition().x) / (64.0f + marble.GetCircle().radius) * 10.0f;
			//tempVelocity = Math::Normalize(tempVelocity);
			marble.SetVelocity(tempVelocity);
		}
	}
}

bool GameLoop(float deltaTime)
{
	return Tick(deltaTime);
}

void Terminate()
{
	if (isServer)
	{
		ServerNetworkManager::StaticTerminate();
	}
	if (isClient)
	{
		ClientNetworkManager::StaticTerminate();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	S::GameApplication myApp;
	ipAdd = lpCmdLine;

	myApp.Initialize(hInstance, "sconfig.json");

	while (myApp.IsRunning())
	{
		myApp.Update(GameLoop);
	}

	Terminate();
	myApp.Terminate();
	return 0;
}