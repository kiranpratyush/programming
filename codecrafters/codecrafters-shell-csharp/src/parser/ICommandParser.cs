using src.parser.commands;
namespace src.parser
{

    interface ICommandParser
    {
        static abstract ICommand Parse(string input);
    }

}


