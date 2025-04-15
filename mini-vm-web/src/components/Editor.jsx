import { useState } from "react";
import axios from "axios";
import Output from "./Output";
import "../components/Editor.css"

export default function Editor() {
  const [code, setCode] = useState("");
  const [output, setOutput] = useState("");

  const runCode = async () => {
    try {
      // setOutput("Inside try");
      const res = await axios.post("http://localhost:5000/run", { code });
      setOutput(res.data.output);
    } catch (err) {
      setOutput("Error executing code.");
    }
  };

  return (
    <div className="editor-container">
      <textarea
        rows={10}
        value={code}
        onChange={(e) => setCode(e.target.value)}
        placeholder="Write your .vm code here..."
      />
      <button className="btn-container" onClick={runCode}>Run</button>
      <Output result={output} />
    </div>
  );
}
