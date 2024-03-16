#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

require 'cgi'

cgi = CGI.new

puts "Content-type: text/html\r\n"
puts "<html>"
puts "<head>"
puts "<meta charset=\"utf-8\">"
puts "<title>Inversor de Nome</title>"
puts "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/bulma/0.9.4/css/bulma.min.css'>"
puts "</head>"
puts "<body>"

puts "<section class='hero is-fullheight is-dark'>"
puts "<div class='hero-body'>"
puts "<div class='container'>"
puts "<h1 class='title is-1 has-text-centered'>Bem-vindo ao Inversor de Nome!</h1>"
puts "<h2 class='subtitle is-3 has-text-centered'>Digite seu nome abaixo:</h2>"

puts "<div class='columns is-centered'>"
puts "<div class='column is-one-quarter'>"
puts "<div class='box'>"
puts "<form method='post'>"
puts "<div class='field'>"
puts "<div class='control'>"
puts "<input class='input' type='text' name='name' placeholder='Seu nome'>"
puts "</div>"
puts "</div>"
puts "<div class='field'>"
puts "<div class='control'>"
puts "<input class='button is-primary is-fullwidth' type='submit' value='Enviar'>"
puts "</div>"
puts "</div>"
puts "</form>"
puts "</div>"
puts "</div>"
puts "</div>"

if cgi.request_method == "POST"
  name = cgi.params["name"].first
  reversed_name = name.reverse
  puts "<h1 class='title is-3 has-text-centered'>Olá, #{reversed_name}!</h1>"
end

puts "</div>"
puts "</div>"
puts "</section>"

puts "</body>"
puts "</html>"
