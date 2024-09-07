#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

alias ls='ls --color=auto'
alias grep='grep --color=auto'
PS1='[\u@\h \W]\$ '

alias hyprcfg="micro ~/.config/hypr/hyprland.conf"

alias dwmcfg="cd ~/dwm/dwm/; micro config.h"
alias vim="micro"
alias dwmpush="git add .; git commit -m auto; git push"
