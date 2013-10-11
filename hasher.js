/**
 * hasher.js by scstauf@gmail.com
 * 
 * description: using Node.js to generate checksums of files.
 *
 * usage:
 *		node hasher.js [digest] [file]		// generate checksum
 *		node hasher.js --digests		// display available algorithms
 *
 * 11 October 2013
 */

var crypto = require('crypto');
var fs = require('fs');

function generate_hash(file, hash) {
	var	cksm = crypto.createHash(hash);

	fs.exists(file, function(exists) {
		if (exists) {
			var strm = fs.ReadStream(file);

			strm.on('data', function(d) {
				cksm.update(d);
			});
			
			strm.on('end', function() {
				var d = cksm.digest('hex');
				console.log(d + '\t' + file);
			});
		} else
			console.log('error:\t\t\'' + file + '\' does not exist.');
	});
}

if (process.argv.length === 4) {
	var file = process.argv[3],
		hash = process.argv[2];
	
	try {
		generate_hash(file, hash);
	} catch (exc) {
		console.log('error:\t\tcould not generate checksum');
		console.log('details:\t' + exc.message);
	}
} else if (process.argv.length === 3) {
	if (process.argv[2] === '--digests')
		console.log('available digests:\n' + crypto.getHashes());
	else
		console.log('invalid parameter: ' + process.argv[2]);
} else {
	console.log('usage:\tnode hasher.js [digest] [file]');
	console.log('\tnode hasher.js --digests');
}
