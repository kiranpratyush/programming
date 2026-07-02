namespace src.utils
{
    public static class FindCommand
    {
        public static string[] getAllCommands(string pathSeparator = ":")
        {
            IEnumerable<string> result = [];
            string? pathValue = Environment.GetEnvironmentVariable("PATH");
            if (!string.IsNullOrEmpty(pathValue))
            {
                var paths = pathValue.Split(pathSeparator);
                foreach (var path in paths)
                {
                    var eachDirResult = TraverseDirectory(path);
                    result = result.Concat(eachDirResult);
                }
            }
            return result.ToArray();

        }
        private static string[] TraverseDirectory(string directory)
        {
            if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(System.Runtime.InteropServices.OSPlatform.Windows))
            {
                throw new PlatformNotSupportedException("Unix file permissions are not supported on Windows.");
            }

            IEnumerable<string> result = [];
            try
            {
                var files = Directory.GetFiles(directory);
                foreach (var file in files)
                {
                    var permissions = File.GetUnixFileMode(file);
                    if (permissions.HasFlag(UnixFileMode.UserExecute))
                    {
                        result = result.Append(Path.GetFileName(file));
                    }
                }
            }
            catch (Exception)
            {

            }
            return result.ToArray();
        }
    }
}