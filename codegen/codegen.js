const generators = [require('./generators/builtin_types')];
const fs = require('fs');
const path = require('path');

const root = path.resolve(__dirname, '..');

function codegen() {
    generators.forEach(g => {
        const codes = g.generate();
        for (let [filePath, code] of codes) {
            fs.writeFileSync(path.join(root, filePath), code);
            console.log(`Generated ${filePath}`);
        }
    })
}

codegen();
