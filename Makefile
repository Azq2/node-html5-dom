all:
	node-gyp configure build
	nodejs index.js
