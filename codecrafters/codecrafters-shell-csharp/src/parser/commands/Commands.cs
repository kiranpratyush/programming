using System.IO.Pipelines;

namespace src.parser.commands
{
    abstract class NotBuiltInCommand
    {
        protected string FindCommand(string pathSeparator, string command)
        {
            string? pathValue = Environment.GetEnvironmentVariable("PATH");

            if (!string.IsNullOrEmpty(pathValue))
            {
                var paths = pathValue.Split(pathSeparator);
                foreach (var path in paths)
                {
                    string result = TraverseDirectory(path, command);
                    if (!string.IsNullOrEmpty(result))
                    {
                        return result;
                    }
                }
            }
            return string.Empty;
        }
        private string TraverseDirectory(string directory, string command)
        {
            if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(System.Runtime.InteropServices.OSPlatform.Windows))
            {
                throw new PlatformNotSupportedException("Unix file permissions are not supported on Windows.");
            }

            try
            {
                var files = Directory.GetFiles(directory);
                foreach (var file in files)
                {
                    if (Path.GetFileName(file) == command)
                    {
                        var permissions = File.GetUnixFileMode(file);
                        if (permissions.HasFlag(UnixFileMode.UserExecute))
                        {
                            return file;
                        }
                    }
                }
            }
            catch (Exception)
            {
            }
            return string.Empty;
        }
    }
    class EchoCommand : ICommand
    {
        private readonly ParseResult _commandResult;
        public EchoCommand(ParseResult result)
        {
            _commandResult = result;
        }
        public void Run()
        {
            var inputString = String.Join(" ", _commandResult.Arguments);
            if (!string.IsNullOrEmpty(_commandResult.StdOutRedirectFile))
            {
                if (_commandResult.StdOutAppend)
                {
                    File.AppendAllText(_commandResult.StdOutRedirectFile, inputString + "\n");
                }
                else
                {
                    File.WriteAllText(_commandResult.StdOutRedirectFile, inputString + "\n");
                }
                return;
            }

            if (!string.IsNullOrEmpty(_commandResult.StdErrRedirectFile))
            {
                if (_commandResult.StdErrAppend)
                {
                    File.AppendAllText(_commandResult.StdErrRedirectFile, "");
                }
                else
                {
                    File.WriteAllText(_commandResult.StdErrRedirectFile, "");
                }
            }
            Console.WriteLine(inputString);

        }
    }

    class CommandNotFound : NotBuiltInCommand, ICommand
    {
        private readonly ParseResult _commandResult;
        private const string pathSeparator = ":";

        public CommandNotFound(ParseResult result)
        {
            _commandResult = result;
        }
        public void Run()
        {
            var commandPath = FindCommand(pathSeparator, _commandResult.CommandName);
            if (!string.IsNullOrEmpty(commandPath))
            {
                try
                {
                    var processInfo = new System.Diagnostics.ProcessStartInfo
                    {
                        FileName = _commandResult.CommandName,
                        RedirectStandardOutput = !string.IsNullOrEmpty(_commandResult.StdOutRedirectFile),
                        RedirectStandardError = !string.IsNullOrEmpty(_commandResult.StdErrRedirectFile),
                        UseShellExecute = false
                    };

                    foreach (var arg in _commandResult.Arguments)
                    {
                        processInfo.ArgumentList.Add(arg);
                    }

                    var process = System.Diagnostics.Process.Start(processInfo);

                    if (process != null)
                    {
                        if (!string.IsNullOrEmpty(_commandResult.StdOutRedirectFile))
                        {
                            string output = process.StandardOutput.ReadToEnd();
                            if (_commandResult.StdOutAppend)
                            {
                                File.AppendAllText(_commandResult.StdOutRedirectFile, output);
                            }
                            else
                            {
                                File.WriteAllText(_commandResult.StdOutRedirectFile, output);
                            }

                        }
                        if (!string.IsNullOrEmpty(_commandResult.StdErrRedirectFile))
                        {
                            string output = process.StandardError.ReadToEnd();
                            if (_commandResult.StdErrAppend)
                            {
                                File.AppendAllText(_commandResult.StdErrRedirectFile, output);
                            }
                            else
                            {
                                File.WriteAllText(_commandResult.StdErrRedirectFile, output);
                            }

                        }
                        process.WaitForExit();
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Error executing {_commandResult.CommandName}: {ex.Message}");
                }
            }
            else
            {
                Console.WriteLine($"{_commandResult.CommandName}: command not found");
            }
        }
    }
    class EmptyCommand : ICommand
    {
        public void Run()
        {
            Console.WriteLine(" ");
        }
    }
    class ExitCommand : ICommand
    {
        public void Run()
        {
            Environment.Exit(0);
        }
    }

    class TypeCommand : NotBuiltInCommand, ICommand
    {
        private readonly ParseResult _commandResult;

        private readonly List<string> builtinCommands;

        private const string pathSeparator = ":";

        public TypeCommand(ParseResult result, List<string> builtincommands)
        {
            _commandResult = result;
            builtinCommands = builtincommands;
        }

        public void Run()
        {
            string inputString = string.Join(" ", _commandResult.Arguments);

            if (builtinCommands.Contains(inputString))
            {
                Console.WriteLine($"{inputString} is a shell builtin");
                return;
            }

            var commandPath = FindCommand(pathSeparator, inputString);
            if (!string.IsNullOrEmpty(commandPath))
            {
                Console.WriteLine($"{inputString} is {commandPath}");
            }
            else
            {
                Console.WriteLine($"{inputString}: not found");
            }
        }
    }

    class PwdCommand : ICommand
    {
        public void Run()
        {
            try
            {
                string path = Directory.GetCurrentDirectory();
                Console.WriteLine(path);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

        }
    }

    class CdCommand : ICommand
    {
        private readonly string input;
        public CdCommand(ParseResult result)
        {
            var inputString = String.Join(" ", result.Arguments);
            if (string.Equals(inputString, "~"))
            {
                string? homePath = Environment.GetEnvironmentVariable("HOME");
                if (!string.IsNullOrEmpty(homePath))
                {
                    input = homePath;
                    return;
                }
            }
            input = inputString;
        }
        public void Run()
        {
            try
            {
                if (Directory.Exists(input))
                {
                    Directory.SetCurrentDirectory(input);

                }
                else
                {
                    Console.WriteLine($"cd: {input}: No such file or directory");
                }

            }
            catch (Exception)
            {

            }

        }
    }

}