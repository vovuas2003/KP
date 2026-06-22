FILE_MANAGER = nautilus

DC = sudo docker compose
SS = sudo systemctl
DS = sudo docker system

SILENT_BG = > /dev/null 2>&1 &

LOGIN_PASSWORD_FILE = login.txt

.DEFAULT_GOAL := help

help: ## Show list of all targets (default)
	@echo "Available targets in Makefile:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "%-10s %s\n", $$1, $$2}'

password: ## Copy password to clipboard
	@sed -n '2p' $(LOGIN_PASSWORD_FILE) | tr -d '\n' | xclip -selection clipboard

on: start up ## Start docker and vpn

off: down stop ## Stop vpn and docker

restart: down up ## Restart vpn container

up: ## Start vpn container
	$(DC) up -d --no-build

down: ## Stop vpn container
	$(DC) down

build: ## Build vpn docker image
	$(DC) build

ps: ## Show container status
	$(DC) ps -a

log: ## Check logs from container
	$(DC) logs -f

status: ## Check docker status
	$(SS) status docker

start: ## Start docker
	$(SS) start docker

stop: ## Stop docker and docker.socket
	$(SS) stop docker docker.socket

check: ## Check docker system statistics
	$(DS) df -v

clean: ## Full clean docker for whole system (!)
	$(DS) prune -a --volumes -f

smb: ## Open samba network share
	@$(FILE_MANAGER) smb://127.0.0.1:4455 $(SILENT_BG)

sftp: ## Open sftp on work computer
	@$(FILE_MANAGER) sftp://work $(SILENT_BG)

.PHONY: help password on off restart up down build ps log status start stop check clean smb sftp
