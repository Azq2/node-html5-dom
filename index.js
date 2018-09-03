var binding = require('./build/Release/html5-dom.node');
console.log(binding);

(async () => {
	var parser = new binding.Parser({
		threads: 0
	});
	
	var tree = parser.parseSync('<div>aaaa</div>');
	
	console.log(tree.parser(), parser, tree.parser() == parser, tree.parser() === parser);
	
	// console.log(new binding.Tree());
})();

process.on('exit', function () {
	console.log("gc");
	global.gc();
});
