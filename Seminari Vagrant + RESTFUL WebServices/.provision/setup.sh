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

#Install Nginx and PHP5 (don't care about existing installations this command is
#not destructive).
echo "Installing Nginx and PHP5...";
sudo apt-get install -y nginx php5-fpm php5-mysql php5-cli > /dev/null 2>&1;

#Change some vulnerable configurations on php.ini
cp /tmp/provision/config_files/php.ini /etc/php5/fpm/php.ini

#Check if MySQL should be installed. If present avoid running the installation
#script.
if [ ! -d /opt/mysql/server-5.6/ ]; then
	#Install MySQL Server 5.6
	bash /tmp/provision/install_mysql_server_5.6.sh
fi

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

#Restart nginx service
sudo service php5-fpm restart;
sudo service nginx restart;
