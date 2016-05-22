#!/bin/bash

#Export non-interactive Environment variable to force installations not to prompt dialogs
export DEBIAN_FRONTEND=noninteractive

# Update the operating system package repositories and keep installed software
#updated.
echo "Updating OS pacakge repositories...";
sudo apt-get update > /dev/null 2>&1;
echo "Upgrading OS installed packages...";
sudo apt-get upgrade -y > /dev/null 2>&1;

#Install build-essentials and base software (GIT & CURL)
sudo apt-get install -y build-essential curl git;

#Install LEMP stack
echo "Installing Nginx and PHP5...";
sudo apt-get install -y nginx php5-fpm php5-mysql php5-cli > /dev/null 2>&1;

#Change some vulnerable configurations on php.ini
cp /tmp/provision/config_files/php.ini /etc/php5/fpm/php.ini

MariaDB=`mysql --user=root --password=development -e "show databases;"`;
if [ "$MariaDB" != "0" ]; then
	export DEBIAN_FRONTEND=noninteractive
	debconf-set-selections <<< 'mariadb-server-5.5 mysql-server/root_password password development'
	debconf-set-selections <<< 'mariadb-server-5.5 mysql-server/root_password_again password development'
	sudo apt-get install -y mariadb-server;
fi

#Create lsmaker database if not exists
mysql --user=root --password=development -e "CREATE DATABASE IF NOT EXISTS lsmaker;";

#Install composer
echo "Installing composer..."
curl -sS https://getcomposer.org/installer | sudo php -- --install-dir=/usr/local/bin --filename=composer;

if [ ! -d /var/www/lsmaker.dev ]; then
	#Create Slim project base structure.
	echo "Creating base Slim Framework structure...";
	mkdir -p /var/www/;
	cd /var/www/;
	composer create-project slim/slim-skeleton lsmaker.dev;
else
  cd /var/www/lsmaker.dev;
  composer install;
fi

rm -rf /var/www/html;

if [ ! -f /etc/nginx/default_host ]; then
	#Re-configure Nginx hosts
	echo "Re-configuring Nginx virtual hosts...";
	rm -rf /etc/nginx/sites-available/*;
	rm -rf /etc/nginx/sites-enabled/*;
	cp /tmp/provision/config_files/default_host /etc/nginx/sites-available/default_host;
	ln -s /etc/nginx/sites-available/default_host /etc/nginx/sites-enabled/default_host;
fi

#Restart Nginx & MariaBD service
sudo service mysql restart
sudo service php5-fpm restart;
sudo service nginx restart;
