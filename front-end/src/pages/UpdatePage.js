import React, { useState } from "react";
import axios from "axios";
import "../styles/App.css";

function UpdatePage() {
  const [updateText, setUpdateText] = useState("");
  const [updateResults, setUpdateResults] = useState([]);
  const [showResults, setShowResults] = useState(false);

  const handleUpdateClick = async (e) => {
    e.preventDefault();
    if (updateText.trim() !== "") {
      try {
        const postData = { url: updateText };

        const response = await axios.post(
          "http://localhost:5000/api/search",
          postData
        );

        setUpdateResults([...updateResults, response.data]);
        setShowResults(true);
        setUpdateText("");
      } catch (error) {
        console.error("Error adding update:", error);
      }
    }
  };

  const handleDeleteClick = async (url) => {
    try {
      await axios.delete(
        `http://localhost:5016/api/delete?url=${encodeURIComponent(url)}`
      );
      setUpdateResults(updateResults.filter((result) => result.url !== url));
    } catch (error) {
      console.error("Error deleting update:", error);
    }
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
        <form onSubmit={handleUpdateClick}>
          <div className="input-container">
            <input
              type="text"
              placeholder="Enter a link to add web"
              className="input-field"
              value={updateText}
              onChange={(e) => setUpdateText(e.target.value)}
            />
            <button type="submit" className="update-button">
              <span role="img" aria-label="search">
                +
              </span>
            </button>
          </div>
        </form>
        {showResults && (
          <div className="update-container">
            {updateResults.map((result, index) => (
              <div key={index} className="update-result">
                <p>
                  <span className="url">{result.url}</span>
                  <button className="update-now-button">Update Now</button>
                  <button
                    className="delete-button"
                    onClick={() => handleDeleteClick(result.url)}
                  >
                    <span role="img" aria-label="delete">
                      🗑️
                    </span>
                  </button>
                </p>
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}

export default UpdatePage;
