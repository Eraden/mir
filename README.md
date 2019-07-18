# Mir

![License](https://img.shields.io/github/license/Eraden/mir.svg?style=flat)
![Version](https://img.shields.io/github/release/Eraden/mir.svg?style=flat)
![Size](https://img.shields.io/github/languages/code-size/Eraden/mir.svg?style=flat)

![mir](https://github.com/Eraden/mir/raw/master/public/mir.jpg)

Simple migration runner

```bash
mir --url setup my_db
mir --url postgres://user:pass@localhost/my_db up
mir --url postgres://user:pass@localhost/my_db down
mir gen table table_name field_name:field_type:default_value field_name:field_type:default_value
mir gen t table_name field_name:field_type:default_value field_name:field_type:default_value
mir g t table_name field_name:field_type:default_value field_name:field_type:default_value
mir gt table_name field_name:field_type:default_value field_name:field_type:default_value
mir up
mir down
```

### Generate table

Field parts are:

* field_name
* field_type - default `text`
* default_value - default empty string

`default_value` if it's a string must be inside `'` characters. `:` character is forbidden.

### Configuration

`.mir`

```ini
database-url=postgres://localhost/mir
migrations-dir=db/migrations
primary-keys=id,uuid
put-timestamps=true
```

* database-url - database connection info
* migrations-dir - migrations files location
* primary-keys - list of fields which will be `PRIMARY KEY UNIQUE` while generating new migration
* put-timestamps - add `created_at timestamp` and `updated_at timestamp` to table

## Requirements

* boost
* libpq-dev

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j10
sudo make install
```
