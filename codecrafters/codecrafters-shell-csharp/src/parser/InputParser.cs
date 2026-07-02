namespace src.parser
{
    public class ParseResult
    {
        public string CommandName { get; set; } = "";
        public string[] Arguments { get; set; } = Array.Empty<string>();
        public string? StdOutRedirectFile { get; set; }
        public bool StdOutAppend { get; set; }
        public string? StdErrRedirectFile { get; set; }

        public bool StdErrAppend { get; set; }

    }

    class InputParser : IInputParser
    {

        public static string[] Parse(string input)
        {
            var args = new List<string>();
            if (string.IsNullOrEmpty(input))
            {
                return args.ToArray();
            }

            var length = input.Length;
            int start = 0;
            string currentArg = "";

            while (start < length)
            {
                char x = input[start];
                if (x.Equals('\''))
                {
                    start++;
                    while (start < length && !input[start].Equals('\''))
                    {
                        currentArg += input[start];
                        start++;
                    }
                    if (start >= length || !input[start].Equals('\''))
                    {
                        throw new InvalidDataException("Invalid input provided");
                    }
                    start++;
                }
                else if (x.Equals('"'))
                {
                    start++;
                    while (start < length && (!input[start].Equals('"')))
                    {
                        if (input[start].Equals('\\'))
                        {
                            start++;
                            if (start >= length) throw new InvalidDataException("Invalid input provided");
                            char value = input[start];
                            if (value.Equals('"') || value.Equals('\\'))
                            {
                                currentArg += value;
                            }
                            else
                            {
                                currentArg += $"\\{value}";
                            }
                            start++;
                            continue;
                        }
                        if (start >= length)
                        {
                            throw new InvalidDataException("Invalid input provided");
                        }
                        currentArg += input[start];
                        start++;
                    }
                    if (start >= length || !input[start].Equals('"'))
                    {
                        throw new InvalidDataException("Invalid input provided");
                    }
                    start++;
                }
                else if (x.Equals('\\'))
                {
                    start++;
                    if (start >= length)
                    {
                        throw new InvalidDataException("Invalid input provided");
                    }
                    currentArg += input[start];
                    start++;
                }
                else if (x.Equals(' '))
                {
                    if (currentArg.Length > 0)
                    {
                        args.Add(currentArg);
                        currentArg = "";
                    }
                    start++;
                }
                else
                {
                    currentArg += input[start];
                    start++;
                }
            }

            if (currentArg.Length > 0)
            {
                args.Add(currentArg);
            }

            return args.ToArray();
        }

        public static ParseResult ParseWithRedirection(string input)
        {
            var result = new ParseResult();

            if (string.IsNullOrEmpty(input))
            {
                return result;
            }

            // Find redirection operators outside of quotes
            int commandEnd = input.Length;
            bool inSingleQuote = false;
            bool inDoubleQuote = false;
            bool escaped = false;

            for (int i = 0; i < input.Length; i++)
            {
                char c = input[i];

                if (escaped)
                {
                    escaped = false;
                    continue;
                }

                if (c == '\\' && !inSingleQuote)
                {
                    escaped = true;
                    continue;
                }

                if (c == '\'' && !inDoubleQuote)
                {
                    inSingleQuote = !inSingleQuote;
                    continue;
                }

                if (c == '"' && !inSingleQuote)
                {
                    inDoubleQuote = !inDoubleQuote;
                    continue;
                }

                if (!inSingleQuote && !inDoubleQuote && c == '>')
                {
                    if (i > 0 && (input[i - 1] == '1' || input[i - 1] == '2'))
                    {
                        // Make sure the digit is not part of a larger token
                        if (i > 1 && !char.IsWhiteSpace(input[i - 2]))
                        {
                            // It's part of a token like "test1>", not a redirection
                            continue;
                        }
                        commandEnd = i - 1;
                    }
                    else
                    {
                        commandEnd = i;
                    }
                    break;
                }
            }

            // Extract and parse command part
            string commandPart = input.Substring(0, commandEnd).TrimEnd();
            var allArgs = Parse(commandPart);
            if (allArgs.Length > 0)
            {
                result.CommandName = allArgs[0];
                result.Arguments = allArgs.Skip(1).ToArray();
            }

            int pos = commandEnd;
            while (pos < input.Length)
            {
                // Skip whitespace
                while (pos < input.Length && char.IsWhiteSpace(input[pos]))
                {
                    pos++;
                }

                if (pos >= input.Length)
                {
                    break;
                }
                bool isStdErr = false;
                bool isAppend = false;

                if (input[pos] == '2')
                {
                    isStdErr = true;
                    pos++;
                    if (pos >= input.Length)
                    {
                        break;
                    }
                }
                else if (input[pos] == '1')
                {
                    isStdErr = false;
                    pos++;
                    if (pos >= input.Length)
                    {
                        break;
                    }
                }

                if (pos < input.Length && input[pos] == '>')
                {
                    pos++;
                    if (pos < input.Length && input[pos] == '>')
                    {
                        isAppend = true;
                        pos++;
                    }
                }
                else
                {
                    // Not a redirection operator, skip
                    pos++;
                    continue;
                }

                // Skip whitespace before filename
                while (pos < input.Length && char.IsWhiteSpace(input[pos]))
                {
                    pos++;
                }

                if (pos >= input.Length)
                {
                    break;
                }

                // Extract filename
                int fileStart = pos;
                int fileEnd = pos;

                if (input[pos] == '\'' || input[pos] == '"')
                {
                    char quote = input[pos];
                    fileEnd++;
                    while (fileEnd < input.Length && input[fileEnd] != quote)
                    {
                        if (input[fileEnd] == '\\' && quote == '"')
                        {
                            fileEnd++; // Skip escaped character
                        }
                        fileEnd++;
                    }
                    if (fileEnd < input.Length)
                    {
                        fileEnd++; // Include closing quote
                    }
                }
                else
                {
                    // Unquoted filename - read until space or another redirection
                    while (fileEnd < input.Length && !char.IsWhiteSpace(input[fileEnd]))
                    {
                        // Stop if we hit another redirection operator
                        if (input[fileEnd] == '>')
                        {
                            break;
                        }
                        if (fileEnd < input.Length - 1 && (input[fileEnd] == '1' || input[fileEnd] == '2'))
                        {
                            if (input[fileEnd + 1] == '>')
                            {
                                break;
                            }
                        }
                        fileEnd++;
                    }
                }

                string fileNamePart = input.Substring(fileStart, fileEnd - fileStart);
                // Parse the filename to handle quotes
                string[] fileNameArr = Parse(fileNamePart);
                string fileName = fileNameArr.Length > 0 ? fileNameArr[0] : "";

                // Set the appropriate redirection
                if (isStdErr)
                {
                    result.StdErrRedirectFile = fileName;
                    result.StdErrAppend = isAppend;
                }
                else
                {
                    result.StdOutRedirectFile = fileName;
                    result.StdOutAppend = isAppend;
                }

                pos = fileEnd;
            }

            return result;
        }

    }
}