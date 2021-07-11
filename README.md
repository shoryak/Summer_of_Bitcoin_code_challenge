# Summer_of_Bitcoin_code_challenge

## Optimal selection problem
- Bitcoin miners construct blocks by selecting a set of **transactions** from their mempool. Each transaction in the mempool:
includes a **fee** which is collected by the miner if that transaction is included in a block
has a **weight** , which indicates the size of the transaction
may have one or more **parent** transactions which are also in the mempool.

- The miner selects an **ordered list** of transactions which have a combined weight below the maximum block weight. Transactions with parent transactions in the
mempool may be included in the list, but only if all of their parents appear before them in the list.

- Naturally, the miner would like to include the transactions that maximize the total fee.


## Theory behind the challenge (Miner Fee)
- amount sent = amount recieved + transaction fee
- Bitcoins design makes it easier for the sender to specify the `transaction fee` than the reciever. This makes sense since, the `transaction fee` is taken from the sender's wallet.
- When a miner creates a `block proposal`, the miner is entitled to specify where all the fees paid by the transactions in that block proposal should be sent. If the proposal results in a valid block that becomes a part of the best block chain, the fee income will be sent to the specified recipient. If a valid block does not collect all available fees, the amount not collected are permanently destroyed
- To select the set of optimal transaction, miner has to solve two problems
  - **Problem1**: Transaction Fee and Size - Knapsack Problem 
  - **Problem2**: Transaction conflicts 

This repo contains code of my program which outputs txids, separated by newlines, which make a valid block(satisfying all constraints), maximizing the fee to the miner.
