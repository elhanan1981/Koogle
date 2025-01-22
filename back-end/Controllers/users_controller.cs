using KoogleApi.Models;
using KoogleApi.Services;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Threading.Tasks;

namespace KoogleApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class SearchController : ControllerBase
    {
        private readonly SearchService _searchService;

        public SearchController(SearchService searchService)
        {
            _searchService = searchService;
        }

        [HttpPost("login")]
        public async Task<IActionResult> Login([FromBody] User loginRequest)
        {
            Console.WriteLine("Received login request with:");
            Console.WriteLine($"Username: {loginRequest?.Username}, Password: {loginRequest?.Password}");

            if (string.IsNullOrEmpty(loginRequest?.Username) || string.IsNullOrEmpty(loginRequest?.Password))
            {
                Console.WriteLine("Username or password is empty");
                return BadRequest("Username and password are required.");
            }

            var user = await _searchService.GetEntryByUsername(loginRequest.Username);
            if (user == null)
            {
                Console.WriteLine("User not found");
                return Unauthorized("Incorrect username or password");
            }

            if (user.Password != loginRequest.Password)
            {
                Console.WriteLine("Password mismatch");
                return Unauthorized("Incorrect username or password");
            }

            Console.WriteLine("Login successful");
            return Ok(new { id = user.Id });
        }

        [HttpPost("signup")]
        public async Task<IActionResult> Signup([FromBody] User signupRequest)
        {
            Console.WriteLine("Received signup request with:");
            Console.WriteLine($"Name: {signupRequest.Username}, Password: {signupRequest.Password}");

            if (string.IsNullOrEmpty(signupRequest.Username) || string.IsNullOrEmpty(signupRequest.Password))
            {
                return BadRequest("Username and password are required.");
            }

            try
            {
                await _searchService.CreateEntry(signupRequest);
                return Ok(new { id = signupRequest.Id });
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error during signup: {ex.Message}");
                return StatusCode(500, "Internal server error");
            }
        }



        [HttpGet]
        public async Task<List<User>> GetAll() => await _searchService.GetAllEntries();

        [HttpGet("{id:length(24)}")]
        public async Task<ActionResult<List<User>>> GetById(string id)
        {
            var job = await _searchService.GetEntryById(id);

            if (job == null)
            {
                return NotFound();
            }
            return new List<User> { job };
        }

        [HttpPut("{id:length(24)}")]
        public async Task<IActionResult> Put(string id, User updatedJob)
        {
            var job = await _searchService.GetEntryById(id);

            if (job is null)
            {
                return NotFound();
            }
            updatedJob.Id = job.Id;
            await _searchService.UpdateEntry(id, updatedJob);

            return NoContent();
        }

        [HttpDelete("{id:length(24)}")]
        public async Task<IActionResult> Delete(string id)
        {
            var job = await _searchService.GetEntryById(id);

            if (job is null)
            {
                return NotFound();
            }
            await _searchService.RemoveEntry(id);
            return NoContent();
        }
    }
}
