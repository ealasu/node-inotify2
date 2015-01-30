fs = require 'fs'
path = require 'path'
temp = require 'temp'
{expect} = require 'chai'
inotify = require('./')

temp.track()

describe 'inotify', ->
  it 'should work', (cb) ->
    dirPath = temp.mkdirSync()
    events = []

    w = new inotify()
    w.addWatch dirPath, inotify.constants.IN_CLOSE_WRITE, (event) ->
      events.push event

    fs.writeFileSync path.join(dirPath, 'a'), 'hello'

    setTimeout ->
      expect(events).to.eql [{
        cookie: 0
        mask: 8
        name: 'a'
      }]
      w.close()
      cb()
    , 200
