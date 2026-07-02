using src.parser.commands;

namespace src.parser
{
    class CommandParser : ICommandParser
    {
        private static readonly List<string> builtinCommands = new List<string> { "echo", "exit", "type", "pwd", "cd" };
        public static ICommand Parse(string? input)
        {
            return ParseMultiOptionCommands(input);

        }
        private static ICommand ParseMultiOptionCommands(string? input)
        {
            if (!string.IsNullOrEmpty(input))
            {
                var parseResult = InputParser.ParseWithRedirection(input);

                return parseResult.CommandName switch
                {
                    "echo" => new EchoCommand(parseResult),
                    "exit" => new ExitCommand(),
                    "type" => new TypeCommand(parseResult, builtinCommands),
                    "pwd" => new PwdCommand(),
                    "cd" => new CdCommand(parseResult),
                    _ => new CommandNotFound(parseResult),
                };
            }
            return new EmptyCommand();

        }
    }

}