# native-api 
[![NPM version][npm-image]][npm-url] [![Build Status](https://travis-ci.org/nachos/native-api.svg?branch=master)](https://travis-ci.org/nachos/native-api) [![Coverage Status][coveralls-image]][coveralls-url]

Provides an abstraction layer over native apis for nachos.


## Install

```bash
$ npm install --save native-api
```


## Usage

```javascript
var nativeApi = require('native-api');
var process = nativeApi.process;
var file = nativeApi.file;

var processes = process.getAllProcesses();
var fileStats = file.getFileStats('c:\test.txt');
```

## API

_(Coming soon)_


## Contributing

In lieu of a formal styleguide, take care to maintain the existing coding style. Add unit tests for any new or changed functionality. Lint and test your code using [gulp](http://gulpjs.com/).


## License

Copyright (c) 2015. Licensed under the MIT license.



[npm-url]: https://npmjs.org/package/native-api
[npm-image]: https://badge.fury.io/js/native-api.svg
[travis-url]: https://travis-ci.org/user/native-api
[travis-image]: https://travis-ci.org/user/native-api.svg?branch=master
[coveralls-url]: https://coveralls.io/r/user/native-api
[coveralls-image]: https://coveralls.io/repos/user/native-api/badge.png
