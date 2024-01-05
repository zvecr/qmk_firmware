INVALID_FEATURES = \
    COMBO \
    DIP_SWITCH_MAP \
    ENCODER_MAP \
    TAP_DANCE \
    VIA \

$(foreach F,$(INVALID_FEATURES),\
	$(if $(filter file, $(origin $(F)_ENABLE)),\
		$(if $(filter yes, $(strip $($(F)_ENABLE))),\
			$(call CATASTROPHIC_ERROR,Invalid keyboard level feature detected - $(F)) \
		) \
	) \
)
