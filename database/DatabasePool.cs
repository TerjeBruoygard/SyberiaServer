using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;

namespace SyberiaDatabase
{
    public class DatabasePool
    {
        private static DatabasePool instance = null;
        public static DatabasePool Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new DatabasePool();
                }

                return instance;
            }
        }

        private ConcurrentDictionary<string, Database> databases = new ConcurrentDictionary<string, Database>();

        public Database GetOrCreate(string name)
        {
            if (name == null || name.Length == 0)
            {
                throw new ApplicationException("Database name is empty.");
            }

            if (name.IndexOfAny(Path.GetInvalidFileNameChars()) >= 0)
            {
                throw new ApplicationException("Database name has invalid characters.");
            }

            return databases.GetOrAdd(name, x => new Database(name));
        }
    }
}
