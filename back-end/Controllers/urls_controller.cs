using KoogleApi.Models;
using KoogleApi.Services;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace KoogleApi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class UrlsController : ControllerBase
    {
        private readonly UrlsService _searchService;

        public UrlsController(UrlsService searchService)
        {
            _searchService = searchService;
        }

        [HttpPost("search")]
        public async Task<ActionResult<List<SearchResult>>> Search([FromBody] Words words)
        {
            if (string.IsNullOrEmpty(words.ListOfWords))
            {
                return BadRequest("Search word is required.");
            }

            try
            {
                var wordsArray = _searchService.SplitStringBySpaces(words.ListOfWords.ToLower()); // Convert to lowercase
                var terms = await _searchService.GetLinksByWords(wordsArray);
                var results = terms.Select(term => new SearchResult
                {
                    Url = term.Key,
                    Occurrences = term.Value
                }).ToList();
                var sortedPages = results.OrderByDescending(p => p.Occurrences).ToList();

                return Ok(sortedPages);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error in Search method: {ex.Message}");
                return StatusCode(500, "Internal server error");
            }
        }

    }
}