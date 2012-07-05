DROP TABLE IF EXISTS `players`;
CREATE TABLE `players` 
(
	`id` INT,
	`name` VARCHAR(20),
	`password` VARCHAR(30),
	`tileset` VARCHAR(20),
	`tx` SMALLINT UNSIGNED,
	`ty` SMALLINT UNSIGNED,
	`map` TINYINT UNSIGNED,
	`x` SMALLINT UNSIGNED,
	`y` SMALLINT UNSIGNED,
	PRIMARY KEY(id)
);

INSERT INTO `players` VALUES (0, 'admin', 'password', 'dg_classm32.gif', 0, 0, 0, 5, 5);
INSERT INTO `players` VALUES (1, 'test', 'test', 'dg_classm32.gif', 1, 0, 0, 5, 6);