console.log('eq.js loaded');

window.MathJax = {
    tex: {
        inlineMath: [['$', '$'], ['\\(', '\\)']],
        displayMath: [['$$', '$$'], ['\\[', '\\]']]
    }
};

console.log('configured delimiters for MathJax');

// Initialize when page loads
window.addEventListener('load', () => {
    console.log('Page loaded, starting initialization...');
    setTimeout(() => {
        console.log('Fetching eqdict.dat...');
        fetch('eqdict.dat')
            .then(response => {
                console.log('Response received:', response.status);
                return response.text();
            })
            .then(datText => {
                console.log('Data received, length:', datText.length);
                console.log('First 200 chars:', datText.substring(0, 200));
                const equations = parseDatFile(datText);
                console.log('Parsed equations count:', equations.length);
                if (equations.length > 0) {
                    console.log('First equation:', equations[0]);
                }
                displayEquations(document.getElementById('equations'), equations);
            })
            .catch(err => {
                console.error('Error loading equations:', err);
            });
    }, 1000);
});

console.log('added window event listener');

function createRow(name, equation) {
    console.log('Creating row for:', name, 'with equation:', equation);
    const tr = document.createElement('tr');
    const tdName = document.createElement('td');
    const tdEquation = document.createElement('td');
    
    // Create a link for the name that goes to the .md file
    const link = document.createElement('a');
    link.href = `${name}.md`;
    link.textContent = name;
    tdName.appendChild(link);
    
    tdEquation.innerHTML = `$${equation}$`;
    console.log('Equation HTML:', tdEquation.innerHTML);
    tr.appendChild(tdName);
    tr.appendChild(tdEquation);
    return tr;
}

// fill in the table with the equations from the list
function displayEquations(table, list) {
    console.log('displayEquations called with table:', table, 'and list length:', list.length);
    const batchSize = 10; // Process 10 equations at a time
    
    function processBatch(startIndex) {
        console.log('Processing batch starting at index:', startIndex);
        const endIndex = Math.min(startIndex + batchSize, list.length);
        
        for (let i = startIndex; i < endIndex; i++) {
            const eq = list[i];
            let name = eq[0];
            let equation = eq[1];
            const row = createRow(name, equation);
            table.appendChild(row);
            console.log('Added row to table, table now has', table.children.length, 'children');
        }
        
        // Process MathJax for this batch
        if (window.MathJax && window.MathJax.typesetPromise) {
            console.log('Calling MathJax.typesetPromise for batch', startIndex, 'to', endIndex-1);
            MathJax.typesetPromise().then(() => {
                console.log(`Processed batch ${startIndex}-${endIndex-1}`);
                
                // Process next batch if there are more equations
                if (endIndex < list.length) {
                    setTimeout(() => processBatch(endIndex), 100);
                } else {
                    console.log('All equations processed');
                }
            }).catch(err => {
                console.error('MathJax error:', err);
            });
        } else {
            console.log('MathJax not available, skipping typeset');
        }
    }
    
    // Start processing with the first batch
    processBatch(0);
}

// Parse the .dat file and convert to JSON format
function parseDatFile(text) {
    console.log('parseDatFile called with text length:', text.length);
    const lines = text.trim().split('\n');
    console.log('Split into', lines.length, 'lines');
    const equations = [];
    
    for (const line of lines) {
        if (line.trim() === '') continue; // Skip empty lines
        if (line.trim().startsWith('#')) continue; // Skip comment lines
        
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
