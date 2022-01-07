define wildcards
	$(foreach n,$(1),$(if $(wildcard $(n)),$(filter $(2),$(wildcard $(n))) $(call $(0),$(n)/*,$(2))))
endef

define dirs
	$(sort $(dir $(call wildcards,$(1),$(2))))
endef
