window.MathJax = {
    tex: {
        inlineMath: [['$', '$'], ['\\(', '\\)']],
        displayMath: [['$$', '$$'], ['\\[', '\\]']]
    }
};

function createRow(name, equation) {
    const tr = document.createElement('tr');
    const tdName = document.createElement('td');
    const tdEquation = document.createElement('td');
    tdName.textContent = name;
    tdEquation.innerHTML = `$$${equation}$$`;
    tr.appendChild(tdName);
    tr.appendChild(tdEquation);
    return tr;
}

// fill in the table with the equations from the list
function displayEquations(table, list) {
    for (const eq of list) {
        let name = eq[0];
        let equation = eq[1];
        const row = createRow(name, equation);
        table.appendChild(row);
    }
    // Trigger MathJax to render the equations
    if (window.MathJax) {
        MathJax.typeset();
    }
}

// Parse the .dat file and convert to JSON format
function parseDatFile(text) {
    const lines = text.trim().split('\n');
    const equations = [];
    
    for (const line of lines) {
        if (line.trim() === '') continue; // Skip empty lines
        
        // Split on first colon followed by whitespace
        const colonIndex = line.indexOf(':');
        if (colonIndex === -1) continue; // Skip lines without colon
        
        const name = line.substring(0, colonIndex).trim();
        let equation = line.substring(colonIndex + 1).trim();
        
        // Don't double backslashes - keep original LaTeX
        equations.push([name, equation]);
    }
    
    console.log('Parsed equations:', equations);
    return equations;
}
