{
  "targets": [
    {
      "target_name": "process",
      "sources": ["src/process.cpp"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    },
    {
      "target_name": "file",
      "sources": ["src/file.cpp"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
