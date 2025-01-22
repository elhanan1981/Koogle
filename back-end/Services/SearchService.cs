using KoogleApi.Models;
using Microsoft.Extensions.Options;
using MongoDB.Driver;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace KoogleApi.Services
{
    public class SearchService
    {
        private readonly IMongoCollection<User> _userCollection;

        public SearchService(IOptions<DataBaseSettings> SearchDatabaseSettings)
        {
            var mongoClient = new MongoClient(SearchDatabaseSettings.Value.ConnectionString);
            var mongoDatabase = mongoClient.GetDatabase(SearchDatabaseSettings.Value.DatabaseName);
            _userCollection = mongoDatabase.GetCollection<User>(SearchDatabaseSettings.Value.UsersCollection);
        }

        public async Task<List<User>> GetAllEntries()
        {
            var result = await _userCollection.Find(_ => true).ToListAsync();
            Console.WriteLine($"Number of documents returned: {result.Count}");
            return result;
        }

        public async Task<User?> GetEntryById(string id) =>
            await _userCollection.Find(x => x.Id == id).FirstOrDefaultAsync();

        public async Task<User?> GetEntryByUsername(string username) =>
            await _userCollection.Find(x => x.Username == username).FirstOrDefaultAsync();

        public async Task CreateEntry(User newJob) =>
                    await _userCollection.InsertOneAsync(newJob);

        public async Task UpdateEntry(string id, User updatedJob) =>
            await _userCollection.ReplaceOneAsync(x => x.Id == id, updatedJob);

        public async Task RemoveEntry(string id) =>
            await _userCollection.DeleteOneAsync(x => x.Id == id);

    }
}