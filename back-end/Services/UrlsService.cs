using KoogleApi.Models;
using Microsoft.Extensions.Options;
using System.Text.RegularExpressions;
using MongoDB.Bson;
using MongoDB.Driver;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace KoogleApi.Services
{
    public class UrlsService
    {
        private readonly IMongoCollection<Term> _searchCollection;

        public UrlsService(IOptions<DataBaseSettings> searchDatabaseSettings)
        {
            var mongoClient = new MongoClient(searchDatabaseSettings.Value.ConnectionString);
            var mongoDatabase = mongoClient.GetDatabase(searchDatabaseSettings.Value.DatabaseName);
            _searchCollection = mongoDatabase.GetCollection<Term>(searchDatabaseSettings.Value.WordsCollection);
        }

        public async Task<Dictionary<string, long>> GetLinksByWords(List<string> ids)
        {
            try
            {
                // Convert all search terms to lowercase
                var lowercaseIds = ids.Select(id => id.ToLowerInvariant()).ToList();

                // Create a case-insensitive filter
                var filter = Builders<Term>.Filter.In("word", lowercaseIds.Select(id => new BsonRegularExpression($"^{Regex.Escape(id)}$", "i")));
                var result = await _searchCollection.Find(filter).ToListAsync();

                Console.WriteLine($"Found {result.Count} documents");

                if (result.Count == 0)
                {
                    Console.WriteLine("No matching documents found");
                    return new Dictionary<string, long>();
                }

                var urlCounts = new Dictionary<string, long>();

                foreach (var term in result)
                {
                    Console.WriteLine($"Processing term: {term.Word}");
                    Console.WriteLine($"Links count: {term.Links.Count}");

                    if (term.Links.Count == 0)
                    {
                        continue;  
                    }

                    foreach (var linkDict in term.Links)
                    {
                        foreach (var kvp in linkDict)
                        {
                            Console.WriteLine($"URL: {kvp.Key}, Count: {kvp.Value}");
                            if (urlCounts.ContainsKey(kvp.Key))
                            {
                                urlCounts[kvp.Key] = Math.Max(urlCounts[kvp.Key], kvp.Value);
                            }
                            else
                            {
                                urlCounts[kvp.Key] = kvp.Value;
                            }
                        }
                    }
                }

                Console.WriteLine($"Final URL count: {urlCounts.Count}");

                return urlCounts;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error in GetLinksByWords: {ex.Message}");
                throw;
            }
        }

        public List<string> SplitStringBySpaces(string input)
        {
            if (string.IsNullOrWhiteSpace(input))
            {
                return new List<string>();
            }

            return input.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries).ToList();
        }
    }
}