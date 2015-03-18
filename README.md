# native-api [![Build Status](https://travis-ci.org/nachos/native-api.svg?branch=master)](https://travis-ci.org/nachos/native-api)

Cross-platform OS API with native modules.


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