const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const { execFile } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(cors());
app.use(bodyParser.text());

// Ensure the programs directory exists
const programsDir = path.join(__dirname,'MiniVM', 'programs');

// POST route to handle code execution
app.post('/run', (req, res) => {
    const inputCode = req.body;

    // Define file path for writing the input VM code
    const filePath = path.join(programsDir, 'loop.vm');
    fs.writeFileSync(filePath, inputCode); // Write the received code to the file

    // Define the path to the vm.exe executable inside the MiniVM folder
    const vmPath = path.join(__dirname,  'MiniVM', 'vm.exe'); // Corrected path

    // Execute the VM code using vm.exe
    execFile(vmPath, [filePath], (error, stdout, stderr) => {
        if (error) {
            // Send error response if execution fails
            console.error('Error executing vm.exe:', error);
            console.error('stderr:', stderr);
            return res.status(500).send(stderr || error.message);
        }
        // Send the output of the execution
        res.json({output:stdout});
    });
});

// Start the server on port 5000
app.listen(5000, () => {
    console.log('Server running on http://localhost:5000');
});