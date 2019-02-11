# Mir

Simple migration runner

```bash
mir --url setup my_db
mir --url postgres://user:pass@localhost/my_db up
mir --url postgres://user:pass@localhost/my_db down
mir gen table table_name field_name:field_type:default_value field_name:field_type:default_value
mir gen t table_name field_name:field_type:default_value field_name:field_type:default_value
mir g t table_name field_name:field_type:default_value field_name:field_type:default_value
mir gt table_name field_name:field_type:default_value field_name:field_type:default_value
```

### Generate table

Field parts are:

* field_name
* field_type - default text
* default_value - default empty string

`default_value` if string must be inside `'` characters. `:` is forbidden.

### Configuration

`.mir`

```ini
database-url=postgres://localhost/mir
migrations-dir=db/migrations
primary-keys=id,uuid
put-timestamps=true
```
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
