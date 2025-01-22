import React from "react";
import "../styles/App.css";
import { useNavigate } from "react-router-dom";

function Home() {
  const navigate = useNavigate();

  const onButtonClick = () => {
    navigate("/login");
  };

  const onSignupClick = () => {
    navigate("/signup");
  };

  return (
    <div className="App">
      <div className="login-container">
        <h1 className="login-heading">welcome to</h1>
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
            type="button"
            className="connect-button"
            onClick={onButtonClick}
            value={"Log in"}
          />
          <input
            type="button"
            className="connect-button"
            onClick={onSignupClick}
            value="Sign Up"
          />
        </form>
      </div>
    </div>
  );
}

export default Home;
