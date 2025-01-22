import React, { useState } from "react";
import "../styles/App.css";
import { useNavigate } from "react-router-dom";
import axios from "axios";
import { useAuth } from "../pages/safeRouting/AuthContext";

function LoginPage() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const navigate = useNavigate();
  const { login } = useAuth();

  const onButtonClick = async () => {
    try {
      const payload = {
        Id: "string",
        Username: username,
        Password: password,
      };
      const response = await axios.post(
        "http://localhost:5000/api/Search/login",
        payload
      );

      if (response.data.id) {
        login();
        navigate("/searchPage");
      } else {
        alert("Login failed");
      }
    } catch (error) {
      console.error("There was an error logging in!", error);
    }
  };

  return (
    <div className="App">
      <div className="login-container">
        <h1 className="login-heading">login to</h1>
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
        <form>
          <input
            type="text"
            placeholder="username"
            value={username}
            className="input-field"
            onChange={(e) => setUsername(e.target.value)}
          />
          <input
            type="password"
            placeholder="password"
            className="input-field"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
          <input
            type="button"
            className="connect-button"
            onClick={onButtonClick}
            value={"Log in"}
          />
        </form>
      </div>
    </div>
  );
}

export default LoginPage;
