all:
	node-gyp configure build
	nodejs --expose-gc index.js
