/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50556
Source Host           : localhost:3306
Source Database       : xx004

Target Server Type    : MYSQL
Target Server Version : 50556
File Encoding         : 65001

Date: 2019-03-26 17:48:47
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for tb_role
-- ----------------------------
DROP TABLE IF EXISTS `tb_role`;
CREATE TABLE `tb_role` (
  `id` bigint(20) NOT NULL DEFAULT '0',
  `user` varchar(64) DEFAULT NULL,
  `server` int(11) DEFAULT NULL,
  `stamp` int(11) DEFAULT NULL,
  `prof` int(11) DEFAULT NULL,
  `name` varchar(64) DEFAULT '',
  `level` int(11) DEFAULT '0',
  `exp` int(11) DEFAULT '0',
  `money` int(11) DEFAULT '0',
  `create_time` bigint(20) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for tb_skill
-- ----------------------------
DROP TABLE IF EXISTS `tb_skill`;
CREATE TABLE `tb_skill` (
  `role_id` bigint(20) NOT NULL,
  `skill_id` int(11) NOT NULL,
  `level` int(11) NOT NULL,
  PRIMARY KEY (`role_id`,`skill_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for tb_user
-- ----------------------------
DROP TABLE IF EXISTS `tb_user`;
CREATE TABLE `tb_user` (
  `name` varchar(64) NOT NULL,
  `create_time` bigint(20) DEFAULT '0',
  `free_time` bigint(20) DEFAULT '0',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Procedure structure for sp_insert_update_role
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_insert_update_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insert_update_role`(IN `in_id` bigint,IN `in_name` varchar(64),IN `in_user` varchar(64),IN `in_server` int,IN `in_stamp` int,IN `in_level` int,IN `in_prof` int,IN `in_exp` int,IN `in_money` int,IN `in_create_time` bigint)
BEGIN 
	INSERT INTO tb_role(id, name, user, server, stamp, prof, level, exp, money, create_time)
	VALUES(in_id, in_name, in_user, in_server, in_stamp, in_prof, in_level, in_exp, in_money, in_create_time)
	ON DUPLICATE KEY UPDATE id=in_id, name=in_name, user=in_user, server=in_server, stamp=in_stamp, prof=in_prof, level=in_level, exp=in_exp, money=in_money, create_time=in_create_time;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_insert_update_skill
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_insert_update_skill`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insert_update_skill`(IN `in_role_id` bigint,IN `in_skill_id` int,IN `in_level` int)
BEGIN 
	INSERT INTO tb_skill(role_id, skill_id, level)
	VALUES(in_role_id, in_skill_id, in_level)
	ON DUPLICATE KEY UPDATE role_id=in_role_id, skill_id=in_skill_id, level=in_level;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_insert_update_user
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_insert_update_user`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insert_update_user`(IN `in_name` varchar(64),IN `in_create_time` bigint,IN `in_free_time` bigint)
BEGIN
	INSERT INTO tb_user(name, create_time, free_time)
	VALUES(in_name, in_create_time, in_free_time)
	ON DUPLICATE KEY UPDATE name=in_name, create_time=in_create_time, free_time=in_free_time;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_select_role
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_role`(IN `in_id` bigint)
BEGIN
	SELECT * FROM tb_role WHERE id = in_id;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_select_role_by_name
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_role_by_name`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_role_by_name`(IN `in_name` varchar(64))
BEGIN
	SELECT * FROM tb_role WHERE name = in_name;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_select_role_skill
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_role_skill`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_role_skill`(IN `in_rid` bigint)
BEGIN
	SELECT * FROM tb_skill WHERE role_id = in_rid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_select_role_stamp
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_role_stamp`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_role_stamp`(IN `in_server` int)
BEGIN
	SELECT MAX(stamp) AS stamp FROM tb_role WHERE server = in_server;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_select_user
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_user`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_user`(IN `in_name` varchar(64))
BEGIN
	SELECT * FROM tb_user WHERE name = in_name;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for sp_select_user_role
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_user_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_user_role`(IN `in_name` varchar(64),IN `in_server` int)
BEGIN
	SELECT * FROM tb_role WHERE user = in_name AND server=in_server;
END
;;
DELIMITER ;
