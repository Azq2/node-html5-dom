#!/usr/bin/env nodejs
var fs = require('fs');

getErrorCodes();

function getErrorCodes() {
	var files = [
		{
			file:		__dirname + '/../third_party/modest/source/modest/myosi.h', 
			prefix:		'MODEST_STATUS_'
		}, 
		{
			file:		__dirname + '/../third_party/modest/source/mycss/api.h', 
			prefix:		'MyCSS_STATUS_'
		}, 
		{
			file:		__dirname + '/../third_party/modest/source/myhtml/myosi.h', 
			prefix:		'MyHTML_STATUS_'
		}
	];
	
	var tmp = "";
	files.forEach(function (cfg) {
		var source = fs.readFileSync(cfg.file).toString(), 
			rex = new RegExp('(' + cfg.prefix + '[\\w\\d_-]+)\\s*=\\s*([a-fx\\d]+)', 'gim');
		
		var m;
		while ((m = rex.exec(source))) {
			if (m[1] == cfg.prefix + "OK")
				continue;
			tmp += "case " + m[1] + ":\n\treturn \"" + m[1] + "\";\n";
		}
		
		fs.writeFileSync(__dirname + "/../src/gen/modest_errors.c", tmp);
		
		console.log(cfg.file + " - error names ok");
	});
}
