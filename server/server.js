const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const { execFile } = require('child_process');  // Use execFile instead of exec
const fs = require('fs');
const path = require('path');

const app = express();
app.use(cors());
app.use(bodyParser.json());

// Ensure the programs directory exists
const programsDir = path.join(__dirname, 'programs');

// POST route to handle code execution
app.post('/run', (req, res) => {
    const inputCode = req.body.code;

    if (typeof inputCode !== 'string') {
        return res.status(400).send('Invalid input code');
    }

    // Define file path for writing the input VM code
    const filePath = path.join(programsDir, 'loop.vm');
    fs.writeFileSync(filePath, inputCode); // Write the received code to the file
    console.log("Code written to file:", filePath);

    // Define the path to the vm.exe executable inside the MiniVM folder
    const vmPath = path.join(__dirname, 'vm.exe'); // Corrected path
    console.log("Executing VM with command:", `"./vm" "${filePath}"`);

    // Execute the VM code using execFile
    execFile(vmPath, [filePath], (error, stdout, stderr) => {
        if (error) {
            console.error('Execution error:', error);
            return res.status(500).json({ output: stderr || error.message });
        }

        // Log output and send response
        console.log("stdout:", JSON.stringify(stdout));
        console.log("stderr:", stderr); // Add stderr log for better debugging
        res.json({ output: stdout || stderr || "No output from VM" });
    });
});

// Start the server on port 5000
app.listen(5000, () => {
    console.log('Server running on http://localhost:5000');
});
