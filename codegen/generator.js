const fs = require('fs');
const path = require('path');

let localIncludeDir = path.resolve(__dirname, '../include');
let localHeaders = loadLocalHeaders();

function loadLocalHeaders(dir = localIncludeDir) {
    const current = fs.readdirSync(dir);
    let headers = [];
    current.forEach(c => {
        if (fs.statSync(path.join(dir, c)).isFile()) {
            headers.push(c);
        } else {
            headers = headers.concat(loadLocalHeaders(path.join(dir, c)).map(h => path.join(c, h)));
        }
    });
    return headers;
}

function isLocalHeader(header) {
    return localHeaders.includes(header);
}

function genInclude(header) {
    if (isLocalHeader(header)) {
        return `#include "${header}"`;
    } else {
        return `#include <${header}>`;
    }
}

function genIncludes(includes) {
    const local = includes.filter(isLocalHeader).sort();
    const external = includes.filter(h => !isLocalHeader(h)).sort();
    return external.map(genInclude).concat(local.map(genInclude)).join('\n');
}

function genDebug(contents = []) {
    return `#if (defined(DONT_DEBUG))
${Array.isArray(contents) ? contents.join('\n') : contents.toString()}
#endif // DONT_DEBUG`;
} 

function genHeader(description = '', headers = [], contents = [], debugContents = []) {
    return `/** @file
 * ${description}
 */
#pragma once

${genIncludes(headers)}

#if (defined(__cplusplus))
extern "C" {
#endif

${genDebug(debugContents)}

${Array.isArray(contents) ? contents.join('\n') : contents.toString()}

#if (defined(__cplusplus))
}
#endif
`;
}

function genSource(description = '', headers = [], contents = [], debugContents = []) {
    return `/** @file
 * ${description}
 */
 
${genIncludes(headers)}

#if (defined(__cplusplus))
extern "C" {
#endif

${genDebug(debugContents)}

${Array.isArray(contents) ? contents.join('\n') : contents.toString()}

#if (defined(__cplusplus))
}
#endif
`;
}

exports.genIncludes = genIncludes;
exports.genDebug = genDebug;
exports.genHeader = genHeader;
exports.genSource = genSource;