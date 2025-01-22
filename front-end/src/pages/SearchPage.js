import React, { useState } from "react";
import "../styles/App.css";
import { useNavigate } from "react-router-dom";
import axios from "axios";

function SearchPage() {
  const [word, setWord] = useState("");
  const [results, setResults] = useState([]);
  const navigate = useNavigate();

  const onSearchClick = async (e) => {
    e.preventDefault();
    if (word.trim() === "") {
      alert("Please enter a word to search.");
      return;
    }
    try {
      const response = await axios.post("http://localhost:5000/Urls/search", {
        ListOfWords: word,
      });
      console.log("first", response.data)
      setResults(response.data);
    } catch (error) {
      console.error("Error during search:", error);
      alert("There was an error during the search.");
    }
  };

  const handleSettingsClick = () => {
    navigate("/Settings");
  };

  return (
    <div className="App">
      <div className="login-container">
        <h1>
          <span className="logo">
            <span className="blue">k</span>
            <span className="red">o</span>
            <span className="yellow">o</span>
            <span className="blue">g</span>
            <span className="green">l</span>
            <span className="red">e</span>
          </span>
        </h1>
        <form onSubmit={onSearchClick}>
          <div className="input-container">
            <input
              type="text"
              placeholder="Enter a field to search"
              className="input-field"
              value={word}
              onChange={(e) => setWord(e.target.value)}
            />
            <button type="submit" className="search-button">
              <span role="img" aria-label="search">
                🔍
              </span>
            </button>
          </div>
        </form>
        {results.length > 0 && (
          <div className="results-container">
            <div className="results-header">
              <div className="url-header"></div>
              <div className="occurrences-header"></div>
            </div>
            {results.map((result, index) => (
              <div key={index} className="search-result">
                <div className="url" > <a href={result.Url}>{result.Url}</a></div>
                <div className="occurrences">{result.Occurrences}</div>
              </div>
            ))}
          </div>
        )}
      </div>
      <button className="settings-button" onClick={handleSettingsClick}>
        <span role="img" aria-label="settings">
          ⚙️
        </span>
      </button>
    </div>
  );
}

export default SearchPage;
