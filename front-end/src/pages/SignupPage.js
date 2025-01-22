import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import axios from "axios";
import { useAuth } from "../pages/safeRouting/AuthContext";

function SignupPage() {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const navigate = useNavigate();
  const { login } = useAuth();

  const onSignupClick = async (e) => {
    e.preventDefault();
    try {
      const payload = {
        Username: username,
        Password: password,
      };
      const response = await axios.post(
        "http://localhost:5000/api/Search/signup",
        payload
      );
      if (response.data.id) {
        login();
        navigate("/searchPage");
      } else {
        console.error("Signup failed");
      }
    } catch (error) {
      console.error("There was an error signing up!", error);
    }
  };

  return (
    <div className="App">
      <div className="login-container">
      <h1 className="login-heading">signup to</h1>
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
        <form onSubmit={onSignupClick}>
          <input
            type="text"
            placeholder="Username"
            className="input-field"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
          />
          <input
            type="password"
            placeholder="Password"
            className="input-field"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />

          <button className="connect-button" type="submit">
            Sign up
          </button>
        </form>
      </div>
    </div>
  );
}

export default SignupPage;
