/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <network/network_api.h>
#include <consensus/connection/service.hpp>
#include <consensus/consensus_service_stub.hpp>
#include <network/peer_communication_stub.hpp>
#include <ordering/ordering_service_stub.hpp>
#include <torii/processor/query_processor_stub.hpp>
#include <torii/processor/transaction_processor_stub.hpp>
#include <torii/torii_stub.hpp>
#include <validation/chain/validator_stub.hpp>
#include <validation/stateful/stub_validator.hpp>
#include <validation/stateless/validator_impl.hpp>

#include <model/model.hpp>
#include <model/model_crypto_provider_impl.hpp>
#include <crypto/crypto.hpp>
#include <ametsuchi/ametsuchi_stub.hpp>

#include "server_runner.hpp"

int main(int argc, char *argv[]) {
  /*
    connection::api::CommandService commandService;
    connection::api::QueryService queryService;
    consensus::connection::SumeragiService sumeragiService;
    ordering::connection::OrderingService orderingService;

    ServerRunner serverRunner("0.0.0.0", 50051, {
        &commandService,
        &queryService,
        &sumeragiService,
        &orderingService
    });
  */
//  iroha::Irohad irohad;
  iroha::ametsuchi::AmetsuchiStub ametsuchi;

  // TODO replace with actual public private keys
  auto seed = iroha::create_seed("some passphrase");
  auto keypair = iroha::create_keypair(seed);
  iroha::model::ModelCryptoProviderImpl crypto_provider(keypair.privkey, keypair.pubkey);

  iroha::validation::StatelessValidatorImpl stateless_validator(crypto_provider);
  iroha::validation::StatefulValidatorStub stateful_validator;
  iroha::validation::ChainValidatorStub chain_validator;
  iroha::ordering::OrderingServiceStub ordering_service;
  iroha::consensus::ConsensusServiceStub consensus_service;
  iroha::network::PeerCommunicationServiceStub peer_communication_service(
    ordering_service,
    consensus_service);
  iroha::torii::TransactionProcessorStub tp(stateless_validator,
                                            crypto_provider);
  iroha::torii::QueryProcessorStub qp(ametsuchi, ametsuchi);

  iroha::torii::ToriiStub torii(tp, qp);
  // shows required order of execution, since callbacks are called synchronously

  iroha::model::Transaction transaction;
  transaction.commands.push_back(std::make_shared<iroha::model::AddPeer>());
  torii.get_transaction({}, transaction);

  return 0;
}
