#include "Command.h"

void Command::Serialize(S::Network::StreamWriter & write) const
{
	write.Write(mCommand);
}

void Command::Deserialize(S::Network::StreamReader & read)
{
	read.Read(mCommand);
}
