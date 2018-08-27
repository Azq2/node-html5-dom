var binding = require('./build/Release/html5-dom.node');
console.log(binding);

(async () => {
	var parser = new binding.Parser({
		threads: 0
	});
	var v = await parser.parseCallback("bla bla", function () {
		console.log("callback");
	});
	console.log(v);
})();

process.on('exit', function () {
	console.log("gc");
	global.gc();
});
