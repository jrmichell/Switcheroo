# Switcheroo 

A small file converter project.

## Setup

Clone jsoncons into the external folder:
```bash
mkdir external
git clone https://github.com/danielaparker/jsoncons.git external/jsoncons
```

## Build and Run

```bash
cmake -B build && cmake --build build
./build/switcheroo <json or csv file path>
```
