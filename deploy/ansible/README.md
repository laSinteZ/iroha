# How to use new ansible playbook

## Inventory (hosts.list)
```
[iroha-nodes]
iroha-ansible-test1 ansible_host=207.148.74.67 ansible_user=root key=0
iroha-ansible-test2 ansible_host=45.77.175.222 ansible_user=root key=1
iroha-ansible-test3 ansible_host=45.32.105.9 ansible_user=root key=2

[locals]
localhost           ansible_connection=local
```

As you can see, basic host field in group contains `hostname`, `ansible_host <ip>`, `ansible_user`, and `key` field. As you might already know, `key` is a node ID in a iroha network. Please set them in the increasing manner from host-to-host.
> Warning: do not violate the order of node IDs as your P2P network will not work in that case

You can add as much hosts as you want.

## Hostvars

As it is said in IPR-1015 (task to fix ansible), there is a need to change port mappings. Now you can do it by creating file `deploy/ansible/playbooks/host_vars/<node hostname>.yml` and setting an option `internal_port: <port number>`. Example:

```bash
cat playbooks/host_vars/iroha-ansible-test3.yml

internal_port: 10002
```
> Note: by default, `internal_port` is set to 10001.


## How it works

First of all, it install python to all your nodes (mentioned in `iroha-nodes` group). 
Then, it generates all files locally !!!(SEE REQUIREMENTS SECTION)!!! and stores them in /tmp/iroha-files directory. List of files generated:
- peers.list
- genesis.block
- nodeX.priv
- nodeX.pub


After the keypairs and genesis block are generated, they are distributed to the remote hosts where iroha is going to be launched.

File `config.sample` is generated during the files delivery stage. 
> Now, only required keypair is beng sent to the remote host, e.g. `node0.priv`, `node0.pub`, `genesis.block`, `config.sample` can be found at `/opt/docker/iroha/conf/` on the remote host.

## Requirements
1) **iroha-cli** must be installed and accessed using PATH variable (e.g. /usr/bin)