var inotify = require('./');

var s = inotify();
s.addWatch('.');
s.on('data', function(e) {
  console.dir(e);
});
