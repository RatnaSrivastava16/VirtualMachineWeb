export default function Output({ result }) {
  return (
    <div className="output-box">
      <h3>Output:</h3>
      <pre>{result}</pre>
    </div>
  );
}
