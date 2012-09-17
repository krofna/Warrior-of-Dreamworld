DROP TABLE IF EXISTS `players`;
CREATE TABLE `players` 
(
	`guid` BIGINT UNSIGNED NOT NULL,
	`username` VARCHAR(20) NOT NULL,
	`password` VARCHAR(30) NOT NULL,
	`email`    VARCHAR(40) NOT NULL,
	`online` TINYINT UNSIGNED DEFAULT 0,
    	`seclevel` TINYINT UNSIGNED DEFAULT 0,
	`tileset` VARCHAR(20) NOT NULL,
	`tx` SMALLINT UNSIGNED NOT NULL,
	`ty` SMALLINT UNSIGNED NOT NULL,
	`mapid` TINYINT UNSIGNED NOT NULL,
	`x` SMALLINT UNSIGNED NOT NULL,
	`y` SMALLINT UNSIGNED NOT NULL,
	PRIMARY KEY(guid)
);

DROP TABLE IF EXISTS `spells`;
CREATE TABLE `spells`
(
    `entry` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `displayid` INT UNSIGNED NOT NULL,
    `effect` SMALLINT UNSIGNED NOT NULL,
    `value` SMALLINT UNSIGNED NOT NULL,
    `cost` SMALLINT UNSIGNED NOT NULL,
    `name` CHAR(192) NOT NULL,
    PRIMARY KEY(entry)
);

INSERT INTO `spells` VALUES (DEFAULT, 1, 0, 10, 10, 'Bolt');
INSERT INTO `spells` VALUES (DEFAULT, 1, 1, 0, 0, 'Quadruple Accel No Interval');

DROP TABLE IF EXISTS `creature_template`;
CREATE TABLE `creature_template`
(
	`entry` INT UNSIGNED NOT NULL,
	`name` CHAR(192) NOT NULL,
	`tileset` VARCHAR(20) NOT NULL,
	`tx` SMALLINT UNSIGNED NOT NULL,
	`ty` SMALLINT UNSIGNED NOT NULL,
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

INSERT INTO `item_template` VALUES(DEFAULT, 1, 1, "Krofna's bag", 1, 1, 36, "The bottom of the crate is leaking. Leaking tears...");

INSERT INTO `creature_template` VALUES (0, 'krofnica', 'dg_classm32.gif', 7, 10, 'npc_krofnica');
INSERT INTO `creature_template` VALUES (1, 'The FullMetal Alchimist', 'dg_classm32.gif', 7, 10, 'npc_the_fullmetal_alchimist');

DROP TABLE IF EXISTS `creature`;
CREATE TABLE `creature`
(
	`guid` BIGINT UNSIGNED NOT NULL,
	`entry` INT UNSIGNED NOT NULL,
	`map` TINYINT UNSIGNED NOT NULL,
	`x` TINYINT UNSIGNED NOT NULL,
	`y` TINYINT UNSIGNED NOT NULL,
	PRIMARY KEY(guid)
);

INSERT INTO `creature` VALUES (UUID_SHORT(), 0, 0, 10, 10);
INSERT INTO `creature` VALUES (UUID_SHORT(), 1, 0, 11, 11);

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
    `objective` CHAR(192) NOT NULL
    PRIMARY KEY(entry)
);

INSERT INTO `quest_template` VALUES(DEFAULT, "Help!", "One of our developers, has gone rogue! Find and eliminate him!", "Kill FullMetal-Alchimist");

DROP TABLE IF EXISTS `character_bags`;
CREATE TABLE `character_bags`
(
    `guid` BIGINT UNSIGNED NOT NULL,
    `owner_guid` BIGINT UNSIGNED NOT NULL,
    `item_id` BIGINT UNSIGNED NOT NULL,
    `idx` TINYINT UNSIGNED NOT NULL,
    PRIMARY KEY(guid)
);
