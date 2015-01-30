{
  'targets': [
    {
      'target_name': 'inotify',
      'include_dirs': ["<!(node -e \"require('nan')\")"],
      'sources': [
        'src/inotify.cc',
      ],
      'cflags_cc!': [
        '-std=c++11',
      ],
    }
  ]
}
