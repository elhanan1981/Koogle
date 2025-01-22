using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;
using System.Collections.Generic;

namespace KoogleApi.Models
{
    public class Term
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string? Id { get; set; }

        [BsonElement("word")]
        public string Word { get; set; } = "";

        [BsonElement("roots_urls")]
        public List<RootUrlInfo> RootsUrls { get; set; } = new List<RootUrlInfo>();

        [BsonElement("links")]
        public List<Dictionary<string, long>> Links { get; set; } = new List<Dictionary<string, long>>();
    }

    public class RootUrlInfo
    {
        [BsonElement("root_url")]
        public string RootUrl { get; set; } = "";

        [BsonElement("links")]
        public List<LinkInfo> Links { get; set; } = new List<LinkInfo>();
    }

    public class LinkInfo
    {
        [BsonElement("url")]
        public string Url { get; set; } = "";

        [BsonElement("count")]
        public int Count { get; set; }
    }

    public class Words
    {
        public string ListOfWords { get; set; } = "";
    }

    public class SearchResult
    {
        public string Url { get; set; }
        public long Occurrences { get; set; }
    }
}