resource "azurerm_resource_group" "temp" {
  name     = "temp-rg"
  location = "West Europe"
}

resource "azurerm_storage_account" "temp" {
  name                     = "chstablestorage1"
  resource_group_name      = azurerm_resource_group.temp.name
  location                 = azurerm_resource_group.temp.location
  account_tier             = "Standard"
  account_replication_type = "LRS"
}

resource "azurerm_storage_table" "temp" {
  name                 = "temptable"
  storage_account_name = azurerm_storage_account.temp.name
}

data "azuread_client_config" "current" {}

resource "azuread_application" "temp" {
  display_name = "temp"
  owners       = [data.azuread_client_config.current.object_id]
}

resource "azuread_service_principal" "temp" {
  client_id                    = azuread_application.temp.client_id
  app_role_assignment_required = false
  owners                       = [data.azuread_client_config.current.object_id]
}

resource "azuread_service_principal_password" "temp" {
  service_principal_id = azuread_service_principal.temp.id
}

output "sp_details" {
  value = {
    clientId: azuread_application.temp.client_id
    password: azuread_service_principal_password.temp.value
    }
  sensitive = true
}

#Storage Table Data Contributor 


terraform {
  required_providers {
    azurerm = {
      source = "hashicorp/azurerm"
    }
  }
}

provider "azurerm" {
  features {}
  subscription_id = "7ada78da-524b-4ba0-b3bb-758cfae32a65"
  tenant_id = "b151cc99-22b2-4927-94bc-457e7425fa7e"
}

