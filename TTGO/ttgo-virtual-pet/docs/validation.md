# Validação na placa

- [ ] Primeiro boot mostra Pip bebê, barras e 10 moedas.
- [ ] GPIO35 muda ação; GPIO0 executa uma única vez por toque.
- [ ] Gestos longos não executam clique adicional.
- [ ] Alimentar consome moeda, brincar ganha moedas, banho melhora higiene.
- [ ] Dormir/acordar altera expressão e recupera energia ao passar um minuto.
- [ ] Estatísticas mostram os cinco atributos e idade.
- [ ] Após um cuidado, aguardar pelo menos 3 segundos, reiniciar e verificar progresso.
- [ ] Novo bichinho pede confirmação; cancelar preserva o estado.
- [ ] Testar por 60 minutos: estágio jovem, sem travamento ou quedas contínuas de heap.

Display, persistência na NVS e gestos físicos precisam da validação após upload. O firmware ainda não foi gravado automaticamente.

## Cuidados animados

- [ ] Alimentar mostra colher e mastigacao; custo de uma moeda mesmo ao tocar repetidamente durante a animacao.
- [ ] Brincar mostra salto e bola; banho mostra espuma e gotas; remedio mostra comprimido e frasco.
- [ ] Acao recusada por moedas, sono ou atributo no maximo nao mostra animacao de sucesso.
- [ ] Apos aproximadamente 3 segundos retorna a expressao normal; cuidados e progresso continuam sendo salvos.

## Versao 2.0

- [ ] Tela inicial mostra nome no topo, pet centralizado e idade em horas/minutos abaixo, sem barras ou menu.
- [ ] GPIO35 abre cuidados; GPIO0 faz carinho; hold0 mostra estatisticas.
- [ ] Atualizar pet 1.x preserva idade, moedas, atributos e sono.
- [ ] Objetivos e recompensa resistem ao reset; resgatar de novo nao duplica moedas.
- [ ] Nome/cor/acessorio salvos; equipar item ja comprado nao cobra novamente.
- [ ] Brilho diminui apos 45 segundos e volta no toque; gestos continuam corretos.
- [ ] Menus voltam a tela limpa apos 20 segundos de inatividade.
- [ ] Evolucao por cuidados recebe comemoracao; humores refletem as necessidades.

- [ ] Trocar nome em Personalizar atualiza o nome na tela inicial; idade aumenta apos um minuto e permanece apos salvamento/reset.
