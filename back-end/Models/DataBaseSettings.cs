namespace KoogleApi.Models;

public class DataBaseSettings
{
    
    public string ConnectionString { get; set; } = null!;

    public string DatabaseName { get; set; } = null!;

    public string UsersCollection { get; set; } = null!;

    public string UrlsCollection { get; set; } = null!;

    public string WordsCollection { get; set; } = null!;


}