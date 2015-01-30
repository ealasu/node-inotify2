fs = require 'fs'
path = require 'path'
temp = require 'temp'
{expect} = require 'chai'
chan = require 'chan'
inotify = require('./')

temp.track()

describe 'inotify', ->
  it 'should work', (cb) ->
    dirPath = temp.mkdirSync()
    ch = chan(1)

    w = new inotify()
    w.addWatch dirPath, inotify.constants.IN_CLOSE_WRITE, (event) ->
      ch(event)

    fs.writeFileSync path.join(dirPath, 'a'), 'hello'

    ch (_, event) ->
      expect(event).to.eql {
        cookie: 0
        mask: 8
        name: 'a'
      }
      w.close()
      cb()
