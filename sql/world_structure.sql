-- World database structure --

DROP TABLE IF EXISTS `players`;
CREATE TABLE `players` 
(
    `guid` BIGINT UNSIGNED NOT NULL,
    `username` VARCHAR(20) NOT NULL,
    `password` VARCHAR(30) NOT NULL,
    `email` VARCHAR(40) NOT NULL,
    `online` TINYINT UNSIGNED DEFAULT 0,
    `seclevel` TINYINT UNSIGNED DEFAULT 0,
    `tileset` VARCHAR(20) NOT NULL,
    `tx` SMALLINT UNSIGNED NOT NULL,
    `ty` SMALLINT UNSIGNED NOT NULL,
    `mapid` INT UNSIGNED NOT NULL,
    `x` SMALLINT UNSIGNED NOT NULL,
    `y` SMALLINT UNSIGNED NOT NULL,
    PRIMARY KEY(guid)
);

DROP TABLE IF EXISTS `spell_template`;
CREATE TABLE `spell_template`
(
    `entry` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `displayid` INT UNSIGNED NOT NULL,
    `effect` SMALLINT UNSIGNED NOT NULL,
    `value` SMALLINT UNSIGNED NOT NULL,
    `cost` SMALLINT UNSIGNED NOT NULL,
    `name` CHAR(192) NOT NULL,
    PRIMARY KEY(entry)
);

DROP TABLE IF EXISTS `creature_template`;
CREATE TABLE `creature_template`
(
    `entry` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `name` CHAR(192) NOT NULL,
    `tileset` VARCHAR(20) NOT NULL,
    `tx` SMALLINT UNSIGNED NOT NULL,
    `ty` SMALLINT UNSIGNED NOT NULL,
    `npcflag` SMALLINT UNSIGNED NOT NULL,
    `maxhealth` INT UNSIGNED NOT NULL,
    `maxpower` INT UNSIGNED NOT NULL,
    `ScriptName` CHAR(192),
    PRIMARY KEY(entry)
);

DROP TABLE IF EXISTS `gameobject_template`;
CREATE TABLE `gameobject_template`
(
    `entry` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `name` CHAR(192) NOT NULL,
    `tileset` VARCHAR(20) NOT NULL,
    `tx` SMALLINT UNSIGNED NOT NULL,
    `ty` SMALLINT UNSIGNED NOT NULL,
    `goflag` SMALLINT UNSIGNED NOT NULL,
    `ScriptName` CHAR(192),
    PRIMARY KEY(entry)
);

DROP TABLE IF EXISTS `item_template`;
CREATE TABLE `item_template`
(
    `entry` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `class` INT UNSIGNED NOT NULL,
    `subclass` INT UNSIGNED NOT NULL,
    `name` CHAR(192) NOT NULL,
    `displayid` INT UNSIGNED NOT NULL,
    `inventory_type` INT UNSIGNED NOT NULL,
    `container_slots` SMALLINT UNSIGNED,
    `description` LONGTEXT,
    PRIMARY KEY(entry)
);

DROP TABLE IF EXISTS `map_template`;
CREATE TABLE `map_template`
(
    `entry` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `size_x` TINYINT UNSIGNED NOT NULL,
    `size_y` TINYINT UNSIGNED NOT NULL,
    `mapflag` TINYINT UNSIGNED NOT NULL,
    `ScriptName` CHAR(192),
    PRIMARY KEY(entry)
);

DROP TABLE IF EXISTS `creature`;
CREATE TABLE `creature`
(
    `guid` BIGINT UNSIGNED NOT NULL,
    `entry` INT UNSIGNED NOT NULL,
    `map` INT UNSIGNED NOT NULL,
    `x` TINYINT UNSIGNED NOT NULL,
    `y` TINYINT UNSIGNED NOT NULL,
    PRIMARY KEY(guid)
);

DROP TABLE IF EXISTS `gameobject`;
CREATE TABLE `gameobject`
(
    `guid` BIGINT UNSIGNED NOT NULL,
    `entry` INT UNSIGNED NOT NULL,
    `map` INT UNSIGNED NOT NULL,
    `x` TINYINT UNSIGNED NOT NULL,
    `y` TINYINT UNSIGNED NOT NULL,
    PRIMARY KEY(guid)
);

DROP TABLE IF EXISTS `items`;
CREATE TABLE `items`
(
    `guid` BIGINT UNSIGNED NOT NULL,
    `owner_guid` BIGINT UNSIGNED NOT NULL,
    `bagid` BIGINT UNSIGNED NOT NULL,
    `slot` SMALLINT UNSIGNED NOT NULL,
    `itemid` BIGINT UNSIGNED NOT NULL,
    PRIMARY KEY(guid)
);

DROP TABLE IF EXISTS `quest_template`;
CREATE TABLE `quest_template`
(
    `entry` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    `title` CHAR(192) NOT NULL,
    `details` CHAR(192) NOT NULL,
    `objective` CHAR(192) NOT NULL,
    PRIMARY KEY(entry)
);

DROP TABLE IF EXISTS `creature_quest`;
CREATE TABLE `creature_quest`
(
    `creature_entry` BIGINT UNSIGNED NOT NULL,
    `quest_entry` BIGINT UNSIGNED NOT NULL
);

DROP TABLE IF EXISTS `character_bags`;
CREATE TABLE `character_bags`
(
    `guid` BIGINT UNSIGNED NOT NULL,
    `owner_guid` BIGINT UNSIGNED NOT NULL,
    `item_id` BIGINT UNSIGNED NOT NULL,
    `idx` TINYINT UNSIGNED NOT NULL,
    PRIMARY KEY(guid)
);

DROP TABLE IF EXISTS `map`;
CREATE TABLE `map`
(
    `id` INT UNSIGNED NOT NULL,
    `entry` INT UNSIGNED NOT NULL,
    PRIMARY KEY(id)
);
