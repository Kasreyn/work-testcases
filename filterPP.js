const fs = require('fs');

function removeLineDirectives(input) {
	const lines	   = input.split('\n');
	const filtered = lines.filter(line => !line.match(/^\s*#\s*(line)?\s*\d+/));
	return filtered.join('\n');
}

function removeStdHeaders(input) {
	const stdHeaderRegex = /\/ usr\/ include | <built - in> | <command - line> |\.h > / ;
	const lines			 = input.split('\n');
	const filtered		 = lines.filter(line => !stdHeaderRegex.test(line));
	return filtered.join('\n');
}

function removeEmptyLines(input) {
	const lines	   = input.split('\n');
	const filtered = lines.filter(line => line.trim().length > 0);
	return filtered.join('\n');
}

function removeNamespaces(input) {
	let insideNamespace = false;
	let braceDepth = 0;
	const lines	= input.split('\n');

	const filtered = lines.filter(line => {
		if (insideNamespace) {
            braceDepth += (line.match(/{/g) || []).length - (line.match(/}/g) || []).length;
            if (braceDepth === 0) {
                insideNamespace = false;
                return false;
            }
            return false;
        }

        if (line.includes('namespace std') || line.includes('namespace boost') || line.includes('namespace __gnu_cxx') || line.includes('extern') || line.includes('__gthread') || line.includes('namespace __pstl') || line.includes('namespace mpl_') || line.includes('namespace __cxxabiv1') /* || line.includes('enum') || line.includes('struct sched_param') || line.includes('struct timex')*/ /* || line.includes('namespace __gnu_debug')*/) {
            insideNamespace = true;
            braceDepth = (line.match(/{/g) || []).length - (line.match(/}/g) || []).length;
/*
            if (braceDepth === 0) {
                insideNamespace = false;
            }
*/
            return false;
        }

		return !insideNamespace;
	});

	return filtered.join('\n');
}

function removePragmaDirectives(input) {
	const lines	   = input.split('\n');
	const filtered = lines.filter(line => !line.match(/^\s*#pragma\s*(line)?\s*/));
	return filtered.join('\n');
}

function removeTypedefStructDirectives(input) {
	let insideNamespace = false;
	let braceDepth = 0;
	const lines	= input.split('\n');

	const filtered = lines.filter(line => {
		if (insideNamespace) {
            braceDepth += (line.match(/{/g) || []).length - (line.match(/}/g) || []).length;
            if (braceDepth === 0) {
                insideNamespace = false;
                return false;
            }
            return false;
        }

        if (line.includes('typedef struct')) {
            insideNamespace = true;
            braceDepth = (line.match(/{/g) || []).length - (line.match(/}/g) || []).length;
            return false;
        }

		return !insideNamespace;
	});

	return filtered.join('\n');
}

function removeTypedefDirectives(input) {
	const lines	   = input.split('\n');
	const filtered = lines.filter(line => !line.match(/^\s*typedef\s*(line)?\s*/));
	return filtered.join('\n');
}

function removeBraceContentAfterNamespace(input) {
	let insideNamespace = false;
	let insideBrace = false;
	let braceDepth = 0;
	const lines	= input.split('\n');

	const filtered = lines.filter(line => {
//		console.log(braceDepth + " : " + line);
        if (insideNamespace) {
            if (!insideBrace && line.includes('{')) {
                insideBrace = true;
            }

            braceDepth += (line.match(/{/g) || []).length - (line.match(/}/g) || []).length;

            if (insideBrace) {
                if (braceDepth === 0) {
                    insideNamespace = false;
                }
            }

            return false;
        }

        if (/^namespace$/.test(line)) {
            insideNamespace = true;
            return false;
        }

		return !insideNamespace;
	});

	return filtered.join('\n');
}

function applyFilters(input) {
	let output = removeLineDirectives(input);
	output	   = removeStdHeaders(output);
	output	   = removeEmptyLines(output);
	output	   = removeNamespaces(output);
	output	   = removePragmaDirectives(output);
	output	   = removeTypedefStructDirectives(output);
	output	   = removeTypedefDirectives(output);
	output     = removeBraceContentAfterNamespace(output);
	return output;
}

const input	 = fs.readFileSync(0, 'utf-8'); // Read from stdin
const output = applyFilters(input);
console.log(output);

/*
const output = removeBraceContentAfterNamespace(input);
console.log(output);
*/
