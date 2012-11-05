#!/bin/sh
mysql -u $1 -p wod_test_db < world_structure.sql
mysql -u $1 -p wod_test_db < wod_test_db.sql
