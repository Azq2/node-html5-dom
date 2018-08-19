var binding = require('./build/Release/html5-dom.node');
console.log(binding);


var parser = new binding.Parser();

(async () => {
	var v = await parser.parse("bla bla", function () {
		console.log("callback");
	});
	console.log(v);
})();
